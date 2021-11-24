import {useEffect, useState} from "react";
import {Button, Col, Form, Modal} from "react-bootstrap";
import {DbCrossSection, DbCurveMember, DbStructuralPoint, SystemLine} from "./DatabaseInterface";
import axios from "axios";
import {FontAwesomeIcon} from "@fortawesome/react-fontawesome";
import {faThumbtack} from "@fortawesome/free-solid-svg-icons";

type DialogAddCurveMemberProps = {
    projectId: string;
    show: boolean;
    onHide: () => void;
    onSubmit: () => void;
}

const DialogAddCurveMember = (props: DialogAddCurveMemberProps) => {

    const [data, setData] = useState<DbCurveMember>({
        id: 0,
        uuid: "",
        name: "",
        systemLine: SystemLine.CENTRE,
        beginNode: {
            id: 0,
            name: "",
            uuid: "",
            coordinate_x: 0,
            coordinate_y: 0,
            coordinate_z: 0,
        },
        endNode: {
            id: 0,
            name: "",
            uuid: "",
            coordinate_x: 0,
            coordinate_y: 0,
            coordinate_z: 0,
        },
        crossSection: {
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
        }
    });
    const [structurePointList, setStructurePointList] = useState<DbStructuralPoint[]>([]);
    const [crossSectionList, setCrossSectionList] = useState<DbCrossSection[]>([]);
    const [selectedBeginNode, setSelectedBeginNode] = useState(0);
    const [selectedEndNode, setSelectedEndNode] = useState(0);
    const [selectedCrossSection, setSelectedCrossSection] = useState(0);
    const [pinned, setPinned] = useState(false);

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        event.preventDefault();
        await axios.post(process.env.REACT_APP_BACKEND + '/curvemember/'
            + props.projectId + '/'
            + structurePointList[selectedBeginNode].id + '/'
            + structurePointList[selectedEndNode].id + '/'
            + crossSectionList[selectedCrossSection].id, data)
        props.onSubmit();
        if (pinned === false)
            props.onHide();
    };

    useEffect(() => {
        axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
            .then(response => setStructurePointList(response.data))
        axios.get(process.env.REACT_APP_BACKEND + '/crosssections/' + props.projectId)
            .then(response => setCrossSectionList(response.data))
    }, [props.show, props.projectId]);

    return (
        <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
            <Modal.Header closeButton>
                <Modal.Title id="contained-modal-title-vcenter">Add new curve member</Modal.Title>
            </Modal.Header>
            <Form onSubmit={handleSubmit}>
                <Modal.Body>
                    <Form.Group as={Col} md="4" controlId="validationCurveMemberName">
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
                    <Form.Group controlId="controlSelectBegin">
                        <Form.Label>from</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedBeginNode(e.target.selectedIndex);
                        }}>
                            {structurePointList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                    <Form.Group controlId="controlSelectEnd">
                        <Form.Label>to</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedEndNode(e.target.selectedIndex);
                        }}>
                            {structurePointList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
                    </Form.Group>
                    <Form.Group controlId="controlSelectCrossSection">
                        <Form.Label>Cross Section</Form.Label>
                        <Form.Control as="select" onChange={(e: any) => {
                            setSelectedCrossSection(e.target.selectedIndex);
                        }}>
                            {crossSectionList.map(e => (<option key={e.id}>{e.name}</option>))}
                        </Form.Control>
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

export default DialogAddCurveMember;
