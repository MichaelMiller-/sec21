import {useEffect, useState} from "react";
import {Button, Col, Form, Modal, Spinner} from "react-bootstrap";
import {DbCrossSection, DbMaterial, SystemLine} from "./DatabaseInterface";
import axios from 'axios';
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddCrossSectionProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddCrossSection = (props: DialogAddCrossSectionProps) => {

    const [data, setData] = useState<DbCrossSection>({
        id: 0,
        uuid: "",
        name: "",
        area: 0,
        systemLine: SystemLine.CENTRE,
        material: {
            id: 0,
            uuid: "",
            name: "",
            E_modulus: 0
        }
    })
    const [materialList, setMaterialList] = useState<DbMaterial[]>([]);
    const [loading, setLoading] = useState(true)
    const [selectedMaterialIndex, setSelectedMaterialIndex] = useState(0);
    const [pinned, setPinned] = useState(false);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/crosssections/' + props.projectId + '/' + materialList[selectedMaterialIndex].id, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    useEffect(() => {
        axios.get(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId)
            .then(response => setMaterialList(response.data))
            .finally(() => setLoading(false))
    }, [props.show, props.projectId]);

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new cross-section</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationCustom01">
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
                    {loading && <Spinner animation="border" role="status"></Spinner>}
                    <Form.Group controlId="controlSelectBegin">
                        <Form.Label>Material</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedMaterialIndex(e.target.selectedIndex);
                        }}>
                            {materialList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                    <Form.Group as={Col} md="4" controlId="validationCustom02">
                        <Form.Label>Area [m2]</Form.Label>
                        <Form.Control
                            required
                            type="text"
                            name="area"
                            value={data.area}
                            onChange={handleInputChange}
                        />
                    </Form.Group>
                </Modal.Body>
                <Modal.Footer>
                    <Button variant="link" onClick={() => {
                        setPinned(!pinned)
                    }}>
                        <FontAwesomeIcon icon={faThumbtack} flip={pinned ? "vertical" : "horizontal"} size="2x"/>
                    </Button>
                    <Button type="submit">Submit</Button>
                    <Button onClick={props.onHide}>Close</Button>
                </Modal.Footer>
            </Form>
        </Modal>
    );
}

export default DialogAddCrossSection;
