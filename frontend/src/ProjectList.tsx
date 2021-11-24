import React, {useEffect, useState} from 'react';
import Col from "react-bootstrap/Col";
import {
    Form, Image,
    Modal,
    Row,
    Spinner, Table,
} from "react-bootstrap";
import {DbProject} from './DatabaseInterface';
import axios from 'axios';
import Header from "./ui/Header";
import NewButton from "./ui/NewButton";
import OpenButton from "./ui/OpenButton";
import {useHistory} from "react-router-dom";
import formatDate from "./formatDate";
import RemoveButton from "./ui/RemoveButton";
import Button from "react-bootstrap/Button";
import LogPalette from './LogPalette';
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faFileExport} from "@fortawesome/free-solid-svg-icons";

type ProjectDialogProps = {
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const ProjectDialog = (props: ProjectDialogProps) => {

    const [data, setData] = useState<DbProject>({
        id: 0,
        name: "",
        title: "",
        createdAt: new Date(),
        modifiedAt: new Date()
    })

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { preventDefault: () => void; }) => {
        event.preventDefault()
        await axios.post(process.env.REACT_APP_BACKEND + '/projects', data)
        props.onSubmit()
        props.onHide()
    };

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new project</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} controlId="validationProjectName">
                        <Form.Label>Name</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="name"
                            value={data.name}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                    </Form.Group>
                    <Form.Group as={Col} controlId="validationProjectTitle">
                        <Form.Label>Title</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="title"
                            value={data.title}
                            onChange={handleInputChange}
                        />
                        <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

type ProjectListItemProps = {
    item: DbProject;
    onDelete: (id: number) => void;
}

const ProjectListItem = (props: ProjectListItemProps) => {

    const history = useHistory();

    const onExportSAF = async () => {
        const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/export_to_saf', {params: {project: props.item.id}});
        const filename = response.data[0].data;
        window.open(process.env.REACT_APP_DOWNLOAD_URL + filename, "_blank");
    }
    return (
        <tr>
            <td><OpenButton onClick={() => (history.push("/project/" + props.item.id))} disabled={false}/></td>
            <td>{props.item.name}</td>
            <td>{props.item.title}</td>
            <td style={{color: "grey"}}>{formatDate(props.item.modifiedAt)}</td>
            <td style={{color: "grey"}}>{formatDate(props.item.createdAt)}</td>
            <td>
                <Button onClick={onExportSAF} variant="link">
                    <Row>
                        <Col><FontAwesomeIcon icon={faFileExport} size="2x" /></Col>
                        <Col>
                            <div className="transparentImage">
                                <Image src="logosaf.png" height={32}></Image>
                            </div>
                        </Col>
                    </Row>
                </Button>
            </td>
            <td><RemoveButton onClick={() => (props.onDelete(props.item.id))}/></td>
        </tr>
    )
}

const ProjectList = () => {

    const [data, setData] = useState<DbProject[]>([])
    const [loading, setLoading] = useState(true)
    const [showNewDialog, setShowNewDialog] = useState(false);
    const [showLog, setShowLog] = useState(false)
    const [errors, setErrors] = useState<string[]>([])
    const [warnings, setWarnings] = useState<string[]>([])

    const pushError = (msg: string) => {
        errors.push(msg)
        setShowLog(true)
    }

    const getData = () => {
        axios.get(process.env.REACT_APP_BACKEND + '/projects')
            .then(response => setData(response.data))
            .finally(() => setLoading(false))
    }

    const onDelete = async (id: number) => {
        const response = await axios.delete(process.env.REACT_APP_BACKEND + '/project/' + id)
        if (response.data.success === false) {
            pushError(response.data.message)
        } else {
            getData()
        }
    }

    useEffect(() => {
        getData()
    }, []);

    if (loading)
        return (<Spinner animation="border" role="status"></Spinner>);

    return (
        <>
            <Header onBack={() => {
            }} disabledBackButton={true} title="List of Projects"/>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false}/>
            <Table borderless hover size="sm">
                <thead>
                <tr>
                    <th></th>
                    <th>Name</th>
                    <th>Description</th>
                    <th>Last modified</th>
                    <th>Created</th>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                {data.map(e => (<ProjectListItem onDelete={onDelete} item={e}/>))}
                </tbody>
            </Table>

            <ProjectDialog
                show={showNewDialog}
                onHide={() => setShowNewDialog(false)}
                onSubmit={getData}
            />

            <LogPalette show={showLog} errorMessages={errors} warningMessages={warnings} onHide={() => {
                setShowLog(false)
                setErrors([])
                setWarnings([])
            }}/>
        </>
    );
}

export default ProjectList
